class ResponseHandler
{
    constructor(){
        this.responses = []; 
        this.device_info = [];
        this.device_status = [];
    }  

    loadDevices(device_info)
    {
        device_info.then( (result) => {
            this.device_info = result; 
        })             
    }

    handleResponse(response)
    {           
        let res = this.createResponse(response)

        this.device_info.forEach(function (device) {            
            if(device.name === res.name)
            {
                this.responses = this.responses.filter(response => (response.name !== response.name))
                this.responses.push(res)    
                //Break exception?
            }
        }.bind(this));     
    }

    createResponse(response)
    {
        let date = new Date;
        let time = date.getTime()

        response = JSON.parse(response)

        let res = {
            name : response.name,
            //address : response.status.address,
            //status : response.status.status,
            //rssi : response.status.rssi,
            //time : time
        } 
        return res;
    }    
}


module.exports = ResponseHandler;