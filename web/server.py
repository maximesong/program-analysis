import web

app = web.auto_application()
render = web.template.render('templates/')

class index(app.page):
	path = '/'
	def GET(self):
		return render.index()

if __name__ == '__main__':
	app.run()


