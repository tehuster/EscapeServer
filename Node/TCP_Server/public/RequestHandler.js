console.log("RequestHandler init..");

function SendRequest() {    
    let deviceName = document.getElementById('deviceName').value;
    let requestType = document.getElementById('requestType').value;
    let actionName = document.getElementById('actionName').value;
    let actionParameter = document.getElementById('actionParameter').value;
    if(actionParameter == "")
    {
      actionParameter = null;
    }
    socket.emit('server', {
                            type: 'request', 
                            command: 'manual', 
                            deviceName: deviceName, 
                            requestType: requestType, 
                            actionName:actionName, 
                            actionParameter:actionParameter
                          });
    console.log(`Requesting: ${requestType} @ ${deviceName}`);    
}
