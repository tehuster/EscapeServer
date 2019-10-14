class ResponseHandler
{
    constructor(){
        this.responses = []; 
        this.device_info = [
            {room: 'Jan', address: 10, name: 'Client101', config: 'test', actions: 'test'},
            {room: 'Jan', address: 20, name: 'Knocker', config: 'test', actions: 'test'}
        ];
    }  

    handleResponse(response)
    {           
        let res = this.createResponse(response)

        this.device_info.forEach(function (device) {            
            if(device.name === res.name)
            {
                this.responses[device.name] = res;
                //Break exception?
            }
        }.bind(this));        
       
        console.log(this.responses);        
    }

    createResponse(response)
    {
        let date = new Date;
        let time = date.getTime()

        response = JSON.parse(response)

        let res = {
            name : response.status.name,
            address : response.status.address,
            status : response.status.status,
            rssi : response.status.rssi,
            time : time
        } 
        return res;
    }
    
}


module.exports = ResponseHandler;