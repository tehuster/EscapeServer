console.log("RequestHandler init..");

function SendRequest() {    
    let requestName = document.getElementById('requestName').value;
    let requestType = document.getElementById('requestType').value;
    socket.emit('server', {type: 'request', requestName: requestName, requestType: requestType});
    console.log(`Requesting: ${requestType} @ ${requestName}`);    
}
