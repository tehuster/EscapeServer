const net = require('net')

class TCP {
    constructor(port = 8080, ip = '192.168.1.249') {        
        this.port = port;
        this.ip = ip;
                
        this.server = net.createServer(this.onClientConnection)

        this.server.listen(this.port, this.host, function () {
            console.log(`TCP_Server started on port ${port} at ${ip}`)
        })
    }
    
    onClientConnection(sock) {
        //Log when a client connnects.
        console.log(`${sock.remoteAddress}:${sock.remotePort} Connected`);
       
        sock.on('data', function (data) {            
            //Log data from the client
            console.log(`${sock.remoteAddress}:${sock.remotePort} Says : ${JSON.parse(data).status.name} `)
            //Send back the data to the client.
            sock.write(`Hello Client, it's me Server`);
        })
        
        sock.on('timeout', function () {
            console.log(`${sock.remoteAddress}:${sock.remotePort} Connection Timeout`);
        })
        
        sock.on('close', function () {
            //console.log(`${sock.remoteAddress}:${sock.remotePort} Terminated the connection`);
        })
        
        sock.on('error', function (error) {
            console.error(`${sock.remoteAddress}:${sock.remotePort} Connection Error ${error}`);
        })
    }
}

module.exports = TCP;

