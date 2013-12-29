import web
import os
import subprocess
import sys
import json

app = web.auto_application()
render = web.template.render('templates/', base='layout')

render2 = web.template.render('templates/')

f = open("sample_code.cpp")
sample_code = f.read()

sample_root = "../samples"

class index(app.page):
    path = '/'
    def GET(self):
        return render.index(sample_code, "", "")

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
        return render.index(code, json.dumps(output_json), output)

class dir(app.page):
    path = '/dir'
    def GET(self):
        listfile = os.listdir('cppcodes')
        return render.mydir(listfile)

class listdir(app.page):
    def POST(self):
        params = json.loads(web.data())
        return self.dir_to_json(params["dir"])

    def GET(self):
        return self.dir_to_json('/')

    def to_sample_dir(self, path):
        return os.path.join(sample_root + path)

    def dir_to_json(self, path):
        entries = os.listdir(self.to_sample_dir(path))
        print(self.to_sample_dir(path))
        print(entries)
        files = []
        dirs = []
        for e in entries:
            p = os.path.join(path, e)
            if os.path.isfile(self.to_sample_dir(p)):
                files.append(e)
            elif os.path.isdir(self.to_sample_dir(p)):
                dirs.append(e)
        web.header('Content-Type', 'application/json')
        return json.dumps({
            'base': path,
            'files': files,
            'dirs': dirs,
            'parent': os.path.normpath(os.path.join(path, os.pardir)),
        })
    
class sources(app.page):
    def GET(self):
        return render2.sources()    

if __name__ == '__main__':
	app.run()


