import web
import os
import subprocess
import sys

app = web.auto_application()
render = web.template.render('templates/')

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
        json = '{"nodes": ["SWH","LMY","ZYL","ZJJ"],"edges": [["ZJJ", "SWH"],["ZJJ", "LMY"],["ZJJ", "ZYL"],["SWH", "LMY"],["SWH", "ZYL"]]}'
        return render.index(output,json)

class dir(app.page):
    path = '/dir'
    def GET(self):
        listfile = os.listdir('cppcodes')
        return render.mydir(listfile)

if __name__ == '__main__':
	app.run()


