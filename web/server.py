import web

app = web.auto_application()
render = web.template.render('templates/')

class index(app.page):
    path = '/'
    def GET(self):
        return render.index("","")
    def POST(self):
        mystring = web.input(code="")
        code = mystring.code;
        json = '{"nodes": ["SWH","LMY","ZYL","ZJJ"],"edges": [["ZJJ", "SWH"],["ZJJ", "LMY"],["ZJJ", "ZYL"],["SWH", "LMY"],["SWH", "ZYL"]]}'
        return render.index(code,json)

if __name__ == '__main__':
	app.run()


