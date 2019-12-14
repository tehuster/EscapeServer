class WebClient {
   constructor(devices, hints, triggers) {
      const express = require("express")
      const path = require('path')
      const bodyParser = require('body-parser')
      const app = express()
      const os = require('os')
      const networkInterfaces = os.networkInterfaces()

      app.use(bodyParser.json())
      app.use(express.static(path.join(__dirname, '/node_modules/bulma/css')))
      app.use(express.static(path.join(__dirname, '/public')))
      app.use(express.static(path.join(__dirname, '/view')))
      app.set('view engine', 'pug')

      this.ip = networkInterfaces.Ethernet[1].address;
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
               res.render('devicess', { devices: data })
            });
      })

      app.get('/devicess', (req, res) => {
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

      app.get('/api', (req, res) => {
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
                     res.json({
                        devices: devices,
                        triggers: triggers
                     })
                  })
            })
      })
      app.get('/test', (req, res) => {
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
                     res.render('layout', {
                        devices: devices,
                        triggers: triggers,
                        server_ip: this.ip
                     })
                  })
            })
      })
   }

   initServer() {
      this.server.listen(3000, this.ip, 0, () => {
         console.log(`Webserver is listening on ${this.ip}`)
      });
   }
}

module.exports = WebClient;



