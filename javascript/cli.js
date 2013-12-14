(function() {
	var fs = require("fs");
	var callgraph = require("./callgraph.js");
	var content = fs.readFileSync("sample/sample.js", { encoding: "utf-8" });
	cg = callgraph.dump(content);
})()
