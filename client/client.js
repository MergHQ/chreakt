const ws = new WebSocket('ws://localhost:3000');
var client = {};

ws.onopen = () => {
    let packet = {
        op: 1,
        data: {
            name: 'Merg' 
        }
    }

    ws.send(JSON.stringify(packet));
}

ws.onmessage = evnt => {
    let data = JSON.parse(evnt.data);
    console.log(data);
}