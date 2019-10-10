class RequestHandler 
{    
    constructor()
    {   
        this.requests = [];
        this.requestType = {
            'status' : 0,
            'data' : 1,
            'config_get' : 2,
            'config_set' : 3,
            'actions' : 4,
        }        
    }    

    addRequest(address, requestType) 
    {   
        //TODO: Check if a request already exists for this address 
        this.requests.push(this.newRequest(address, requestType))    
    }

    async checkForRequest(address)
    {       
        return JSON.stringify(this.requests.filter(request => (request.address === address)))
    }

    removeRequest(id) 
    {
        //TODO: We need to check if this works
        this.requests = this.requests.filter(request => (request.id !== id)) //find for first entry
    }

    newRequest(address, requestType)
    {
        let request =
        {
            id : this.id = Math.random().toString(32).substr(2, 8),
            address : address,
            requestType : requestType
        }
        return request;
    }

    clearAllRequests()
    {
        console.log('Clearing all requests...')        
        this.requests = [];
    }
}

module.exports = RequestHandler;