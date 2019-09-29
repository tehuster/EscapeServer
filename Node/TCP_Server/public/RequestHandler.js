console.log("RequestHandler init..");

function SendRequest() {    
    let requestAddress = document.getElementById('requestAddress').value;
    let requestType = document.getElementById('requestType').value;
    socket.emit('server', {type: 'request', requestAddress: requestAddress, requestType: requestType});
    console.log(`Requesting: ${requestType} @ ${requestAddress}`);    
}
