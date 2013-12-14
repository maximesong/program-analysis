var acorn = require("./acorn/acorn.js");
var walk = require("./acorn/util/walk.js");


dump = function dump(content) {
	root = acorn.parse(content, {locations:true});
	walk.simple(root, {
		FunctionExpression: function(node) {
			console.log("Exp");
			console.log(node);
		}
	});
	return acorn.parse(content); 
}

exports.dump = dump
