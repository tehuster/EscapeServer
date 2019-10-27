console.log("RequestHandler init..");

function SendRequest() {    
    let requestName = document.getElementById('requestName').value;
    let requestType = document.getElementById('requestType').value;
    let actionName = document.getElementById('actionName').value;
    let actionParameter = document.getElementById('actionParameter').value;
    socket.emit('server', {
                            type: 'request', 
                            requestName: requestName, 
                            requestType: requestType, 
                            actionName:actionName, 
                            actionParameter:actionParameter
                          });
    console.log(`Requesting: ${requestType} @ ${requestName}`);    
}
