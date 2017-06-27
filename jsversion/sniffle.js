var sniffle = (function(){
    var t = 0;
    global =  window;
    "LIST_EXP, STR_EXP, VAR_EXP, CONST_EXP, CFUNC_EXP, FUNC_EXP".split(",").map(function(x){
	x = x.trim();
	global[x] = t++;
    });

    function anyStr(exp){
	if(exp == null)
	    return "NULL";
	switch(exp.type){
	case LIST_EXP:
	    return "(" + exp.data.map(anyStr).join(" ") + ")";
	case STR_EXP:
	    return '"' + exp.data + '"';
	case VAR_EXP:
	    return exp.data;
	case CONST_EXP:
	    return exp.data + "";
	case CFUNC_EXP:
	    return "CFUNC_EXP";
	case FUNC_EXP:
	    return "lambda";
	}
    }

    function printAny(exp){
	console.log(anyStr(exp));
    }

    function expression(type, data){
	this.type = type;
	this.data = data;
    }

    function pushStr(inputhandled, parent, data,  i,  lastpos, type){
	var tmp;
	var p;
	var exp;
	if(!inputhandled){
	    tmp = (i-lastpos);
	    p = lastpos;
	    exp = new expression(type, data.substr(p, tmp));
	    parent.data.push(exp);
	}
	return true;
    }

    function seekEndOfStr(str, i){
	var c, p = i;
	while(p < str.length && (c = str[p++]) && c != '\"'){
	    if(c == '\\' && (p+1 < str.length))
		++p;
	}
	return p-i;
    }

    function pushInt( parent,  data, i){
	var p = i, c;
	++p;
	while(p < data.length && (c = data[p++]) && c >= '0' && c <= '9');
	var len = p - i - 1;
	parent.data.push(new expression(CONST_EXP, parseInt(data.substr(i, len))));
	return len - 1;
    }

    function parse(data){
	var parent = new expression(LIST_EXP, []);
	var tmp;
	var type = VAR_EXP;
	var parents = [];
	var lastpos = 0;
	var inputhandled = true;
	for(var i = 0; i < data.length; ++i){
	    if(data[i] == '('){
		inputhandled = pushStr(inputhandled, parent, data, i, lastpos, type);
		parents.push(parent);
		tmp = new expression(LIST_EXP, []);
		parent.data.push(tmp);
		parent = tmp;
	    }else if(data[i] == ')'){
		inputhandled = pushStr(inputhandled, parent, data, i, lastpos, type);
		parent = parents.pop();
	    }else if(data[i] == ' ' || data[i] == '\n' || data[i] == '\t'){
		inputhandled = pushStr(inputhandled, parent, data, i, lastpos, type);
	    }else if(inputhandled){
		inputhandled = false;
		lastpos = i;
		if(data[i] == '"'){
		    type = STR_EXP;
		    ++lastpos;
		    ++i;
		    i = i + seekEndOfStr(data, i) - 1;
		    inputhandled = pushStr(inputhandled, parent, data, i, lastpos, type);
		}else if ((data[i] >= '0' && data[i] <= '9') || (data[i] == '-' && i + 1 < data.length && (data[i + 1] >= '0' && data[i + 1] <= '9'))){
		    type = CONST_EXP;
		    i += pushInt(parent, data, i);
		    inputhandled = true;
		}else{
		    type = VAR_EXP;
		}
	    }
	}

	if(parents.length > 0){
	    return null;
	}else{
	    return parent;
	}
    }

    function copyExpression(any){
	if(any == null)
	    return null;
	return new expression(any.type, any.data);
    }

    function evalList(list, env, args){
	var list = list.slice();
	for(var i = 0; i < list.length; ++i){
	    list[i] = evalAST(list[i], env, args);
	}
	return list;
    }

    function evalAST(prog, env, args){
	var temp, temp2, ret;
	var tlist;
	var it;
	if(prog == null) return null;
	switch(prog.type){
	case VAR_EXP:
	    if(args != null){
		it = args[prog.data];
		if(it){
		    return copyExpression(it);
		    break;
		}
	    }
	    if(env != null){
		it = env[prog.data];
		if(it){
		    return copyExpression(it);
		    break;
		}
	    }
	    return null;
	    break;
	case LIST_EXP:
	    temp = prog.data[0];
	    if(temp == null)
		break;
	    temp = evalAST(temp, env, args);
	    if(temp != null){
		tlist = prog.data.slice(1);
		temp2 = new expression(LIST_EXP, tlist);
		if(temp.type == CFUNC_EXP){
		    ret = temp.data(temp2, env, args);
		}else if(temp.type == FUNC_EXP){
		    ret = temp.data.exec(temp2, env);
		}else{
		    ret = null;
		}
		return ret;
	    }
	    break;
	default:
	    return copyExpression(prog);
	    break;
	}
	return null;
    }

    function addFunc(arglist, env, args){
	var temp;
	var list = evalList(arglist.data, env, args);
	var sum = 0;
	for(var i = 0; i < list.length; ++i){
	    temp = list[i];
	    if(temp != null && temp.type == CONST_EXP){
		sum += temp.data;
	    }
	}
	return new expression(CONST_EXP, sum);
    }

    function geFunc(arglist, env, args){
	if(arglist.data.length != 2)
	    return null;
	var list = evalList(arglist.data, env, args);
	if(!list[0] || !list[1])
	    return null;
	return new expression(CONST_EXP, (list[0].type == CONST_EXP && list[1].type == CONST_EXP && list[0].data >= list[1].data) ? 1 : 0);
    }

    function gtFunc(arglist, env, args){
	if(arglist.data.length != 2)
	    return null;
	var list = evalList(arglist.data, env, args);
	if(!list[0] || !list[1])
	    return null;
	return new expression(CONST_EXP, (list[0].type == CONST_EXP && list[1].type == CONST_EXP && list[0].data > list[1].data) ? 1 : 0);
    }

    function ltFunc(arglist, env, args){
	if(arglist.data.length != 2)
	    return null;
	var list = evalList(arglist.data, env, args);
	if(!list[0] || !list[1])
	    return null;
	return new expression(CONST_EXP, (list[0].type == CONST_EXP && list[1].type == CONST_EXP && list[0].data < list[1].data) ? 1 : 0);
    }

    function eqFunc(arglist, env, args){
	if(arglist.data.length != 2)
	    return null;
	var list = evalList(arglist.data, env, args);
	if(!list[0] || !list[1])
	    return new expression(CONST_EXP, list[0] == list[1] ? 1 : 0);
	return new expression(CONST_EXP, (list[0].type == list[1].type && list[0].data == list[1].data) ? 1 : 0);
    }

    function leFunc(arglist, env, args){
	if(arglist.data.length != 2)
	    return null;
	var list = evalList(arglist.data, env, args);
	return new expression(CONST_EXP, (list[0].type == CONST_EXP && list[1].type == CONST_EXP && list[0].data <= list[1].data) ? 1 : 0);
    }


    function beginFunc(arglist, env, args){
	var list = evalList(arglist.data, env, args);
	return list[list.length - 1];
    }

    function printAnyFunc(arglist, env, args){
	var list = evalList(arglist.data, env, args);
	printAny(new expression(LIST_EXP, list));
	return null;
    }

    function truthiness(exp){
	if(exp == null)
	    return 0;
	else if(exp.type == CONST_EXP)
	    return exp.data ? 1 : 0;
	return 1;
    }

    function ifFunc(arglist, env, args){
	var temp, res = null;
	var list = arglist.data;
	if(list.length != 3){
	    return null;
	}
	if(truthiness(evalAST(list[0], env, args))){
	    res = evalAST(list[1], env, args);
	}else{
	    res = evalAST(list[2], env, args);
	}

	return res;
    }

    function LambdaFunc(vars, code){
	this.prog = code;
	this.vars = vars.data.map(function(x){
	    return x.data;
	});
	this.args = {};
    }

    LambdaFunc.prototype.exec = function(arglist, env){
	var args = this.args;
	vals = evalList(arglist.data, env, args);
	var min = Math.min(vals.length, this.vars.length);
	for(var i = 0; i < min; ++i){
	    args[this.vars[i]] = vals[i];
	}
	return evalAST(this.prog, env, args);
    };

    function lambdaMakerFunc(arglist, env, args){
	var temp, res = null;
	var list = arglist.data;
	if(list.length != 2){
	    return null;
	}
	var v = list[0].data;
	if(v.constructor != ([]).constructor)
	    return null;
	for(var i = 0; i < v.length; ++i){
	    if(v[i].data.constructor != ("").constructor)
		return null;
	}
	return new expression(FUNC_EXP, new LambdaFunc(copyExpression(list[0]), copyExpression(list[1])));
    }

    function setFunc(arglist, env, args){
	var temp, res = null;
	var list = arglist.data;
	if(list.length != 2){
	    return null;
	}
	var v = list[0].data;
	if(v.constructor != ("").constructor)
	    return null;
	env[v] = evalAST(list[1], env, args);
	return null;
    }

    function setLocalFunc(arglist, env, args){
	console.log("hi?",arglist);
	var temp, res = null;
	var list = arglist.data;
	if(list.length != 2){
	    return null;
	}
	var v = list[0].data;
	if(v.constructor != ("").constructor)
	    return null;
	args[v] = evalAST(list[1], env, args);
	return null;
    }

    function orFunc(arglist, env, args){
	var list = arglist.data;
	var i =0;
	var ret = 0;
	while(i < list.length && (ret = truthiness(evalAST(list[i], env, args))) == 0){
	    ++i;
	}
	
	return new expression(CONST_EXP, ret);
    }

    function andFunc(arglist, env, args){
	var list = arglist.data;
	var i = 0;
	var ret = 1;
	while(i < list.length && (ret = truthiness(evalAST(list[i], env, args))) == 1){
	    ++i;
	}
	return new expression(CONST_EXP, ret);
    }

    function whileFunc(arglist, env, args){
	var list = arglist.data;
	var i = 0;
	var ret = null;
	var car = list[0];
	list = list.slice(1);
	while(truthiness(evalAST(car, env, args)) == 1){
	    ret = evalList(arglist.data, env, args);
	    ret = ret[ret.length - 1];
	}
	return ret;
    }

    function subtractFunc(arglist, env, args){
	var temp;
	var list = evalList(arglist.data, env, args);
	var sum = 0;
	var first = arglist.data.length > 1;
	for(var i = 0; i < list.length; ++i){
	    temp = list[i];
	    if(temp != null && temp.type == CONST_EXP){
		if(first){
		    sum += temp.data;
		    first = false;
		}else{
		    sum -= temp.data;
		}
	    }
	}
	return new expression(CONST_EXP, sum);

    }

    function multiplyFunc(arglist, env, args){
	var temp;
	var list = evalList(arglist.data, env, args);
	var sum = 1;
	for(var i = 0; i < list.length; ++i){
	    temp = list[i];
	    if(temp != null && temp.type == CONST_EXP){
		sum *= temp.data;
	    }
	}
	return new expression(CONST_EXP, sum);
    }

    function divideFunc(arglist, env, args){
	var temp;
	var list = evalList(arglist.data, env, args);
	var sum = 0;
	var first = arglist.data.length > 1;
	for(var i = 0; i < list.length; ++i){
	    temp = list[i];
	    if(temp != null && temp.type == CONST_EXP){
		if(first){
		    sum = temp.data;
		    first = false;
		}else if(temp.data != 0){
		    sum /= temp.data;
		}else{
		    return null;
		}
	    }
	    sum |= 0;
	}
	return new expression(CONST_EXP, sum);
    }

    function run(code){
	var prog = parse(code);
	var ENV = {
	    "or":new expression(CFUNC_EXP, orFunc),
	    "and":new expression(CFUNC_EXP, andFunc),
	    "if":new expression(CFUNC_EXP, ifFunc),
	    "local":new expression(CFUNC_EXP, setLocalFunc),
	    "set":new expression(CFUNC_EXP, setFunc),
	    "lambda":new expression(CFUNC_EXP, lambdaMakerFunc),
	    "pprint":new expression(CFUNC_EXP, printAnyFunc),
	    "begin":new expression(CFUNC_EXP, beginFunc),
	    "while":new expression(CFUNC_EXP, whileFunc),
	    
	    "+":new expression(CFUNC_EXP, addFunc),
	    "-":new expression(CFUNC_EXP, subtractFunc),
	    "*":new expression(CFUNC_EXP, multiplyFunc),
	    "/":new expression(CFUNC_EXP, divideFunc),
	    ">":new expression(CFUNC_EXP, gtFunc),
	    "=":new expression(CFUNC_EXP, eqFunc),
	    "<":new expression(CFUNC_EXP, ltFunc),
	    ">=":new expression(CFUNC_EXP, geFunc),
	    "<=":new expression(CFUNC_EXP, leFunc)
	};

	for(var i = 0; i < prog.data.length; ++i){
	    temp = prog.data[i];
	    printAny(temp);
	    temp = evalAST(temp, ENV, null);
	    printAny(temp);
	}
    }
    return {run:run, parse:parse};
})();

sniffle.run("(+ 1 (+ 2 3) )");

