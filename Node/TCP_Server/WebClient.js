class WebClient {
   constructor() {
      const express = require("express")
      const path = require('path')
      const bodyParser = require('body-parser')
      const app = express()
      app.use(bodyParser.json())
      app.use(express.static(path.join(__dirname, '/node_modules/bulma/css')))
      app.use(express.static(path.join(__dirname, '/public')))
      app.use(express.static(path.join(__dirname, '/view')))
      app.set('view engine', 'pug')
      this.server = require('http').Server(app)
      this.initRoutes(app)
      this.initServer()
   }

   initRoutes(app)
   {
      app.get('/', (req, res) => {
         res.render('index')
      });
      app.get('/devices', (req, res) => {
         res.render('devices')
      });
   }

   initServer()
   {
      this.server.listen(3000, '127.0.0.1', 0, () => {
         console.log('Webserver is listening on 3000')
      });
   }
}

module.exports = WebClient;



