var acorn = require("./acorn/acorn.js");
var walk = require("./acorn/util/walk.js");


dump = function dump(content) {
	root = acorn.parse(content, {locations:true});
	var ci = [],
	    ei = [],
	    ui = [],
	    g = [];
	walk.simple(root, {
		Program: function(node) {
			//console.log(node.body);
		},
		FunctionExpression: function(node) {
			//console.log("Exp");
			//console.log(node);
		},

		CallExpression: function(node) {
			//console.log("Call");
			if (node.callee.type === "FunctionExpression") {
				ci.push({
					caller: node.loc,
					callee: node.callee.loc
				});
				console.log(node);
			}
		}
	});
	console.log(ci);
	walk.simple(root, {
		CallExpression: function(node) {
			//console.log("Call");
			if (node.callee.type === "FunctionExpression") {
				ci.push({
					caller: node.loc,
					callee: node.callee.loc
				});
				console.log(node);
			}
		}
	});
	return acorn.parse(content); 
}

exports.dump = dump
