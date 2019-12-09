class WebClient {
   constructor(devices, hints, triggers) {
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
      this.devices = devices;
      this.triggers = triggers;
      this.hints = hints;
   }

   initRoutes(app) {
      app.get('/', (req, res) => {
         res.render('index')
      })

      app.get('/hints', (req, res) => {
         this.hints.get()
            .then(data => res.render('hints', { hints: data }))
      })

      app.get('/devices', (req, res) => {
         this.devices.findAll({
            include: ['actions', 'configs', 'events']
         })
            .then((data) => {
               res.render('devices', { devices: data })
            });
      })

      app.get('/triggers', (req, res) => {
         let devices;
         let triggers;
         this.devices.findAll({
            include: ['actions', 'configs', 'events']
         })
            .then((device_data) => {
               devices = device_data;
               this.triggers.findAll()
                  .then((trigger_data) => {
                     triggers = trigger_data;
                     res.render('triggers', { 
                        devices: devices, 
                        triggers: triggers 
                     })
                  })
            })
      })
   }

   initServer() {
      this.server.listen(3000, '127.0.0.1', 0, () => {
         console.log('Webserver is listening on 3000')
      });
   }
}

module.exports = WebClient;



