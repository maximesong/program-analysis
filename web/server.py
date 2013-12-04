import web
import os
import subprocess
import sys
import json

app = web.auto_application()
render = web.template.render('templates/', base='layout')

class index(app.page):
    path = '/'
    def GET(self):
        return render.index("","")
    def POST(self):
        mystring = web.input(code="")
        code = mystring.code;
        filename = "myfile.cpp"
        file_object = open(filename,'w')
        file_object.write(code)
        file_object.close()
        proc = subprocess.Popen(["../pa","myfile.cpp"],stdout=subprocess.PIPE)
        proc.wait()
        output=proc.stdout.read()
	input_json = json.loads(output);
	output_json = {
			"nodes": [
				],
			"edges": [
			],
			};
	for func in input_json["functions"]:
		output_json["nodes"].append(func)
	calls = input_json["calls"]
	for caller in calls:
		for callee in calls[caller]:
			output_json["edges"].append([ caller, callee ])
	print output_json
        return render.index(output, json.dumps(output_json))

class dir(app.page):
    path = '/dir'
    def GET(self):
        listfile = os.listdir('cppcodes')
        return render.mydir(listfile)

if __name__ == '__main__':
	app.run()


