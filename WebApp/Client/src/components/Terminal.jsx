import React, { useState, useEffect } from 'react';
import axios from 'axios';
import '../styles/Terminal.css';

const Terminal = () => {
  const [keystrokes, setKeystrokes] = useState("");
  const [showRaw, setShowRaw] = useState(true);

  useEffect(() => {
    const storedKeystrokes = localStorage.getItem('keystrokes');
    if (storedKeystrokes) {
      setKeystrokes(storedKeystrokes);
      console.log('Previous keystrokes loaded from local storage:', storedKeystrokes);
    }

    const fetchKeystrokes = () => {
      axios.post('http://localhost:3000/getStrokes')
        .then(response => {
          const newKeystrokes = response.data.keystrokes;
          console.log('New keystrokes received:', newKeystrokes);
          setKeystrokes(prevKeystrokes => {
            const updatedKeystrokes = prevKeystrokes + newKeystrokes;
            localStorage.setItem('keystrokes', updatedKeystrokes);
            return updatedKeystrokes;
          });
        })
        .catch(error => console.error("Error fetching keystrokes:", error));
    };

    const interval = setInterval(fetchKeystrokes, 1000);
    return () => clearInterval(interval);
  }, []);

  const downloadKeystrokes = () => {
    const content = showRaw ? keystrokes : filteredKeystrokes();
    const element = document.createElement("a");
    const file = new Blob([content], { type: 'text/plain' });
    element.href = URL.createObjectURL(file);
    element.download = "keystrokes.txt";
    document.body.appendChild(element);
    element.click();
  };

  const toggleKeystrokeDisplay = () => {
    setShowRaw(!showRaw);
  };

  const filteredKeystrokes = () => {
    const regex = /\[.*?\]/g;
    const withoutBrackets = keystrokes.replace(regex, '');
    const finalRegex = /[a-zA-Z0-9!@#$%^&*(),.?":{}|<> ]/g;
    return withoutBrackets.match(finalRegex)?.join('') || "";
  };

  return (
    <div className="terminal">
      <div className="terminal-header">
        <h2>Terminal</h2>
        <button className="download-btn" onClick={downloadKeystrokes}>Download</button>
        <button className="toggle-btn" onClick={toggleKeystrokeDisplay}>
          {showRaw ? 'Show Filtered Keystrokes' : 'Show Raw Keystrokes'}
        </button>
      </div>
      <div className="terminal-content">
        {showRaw ? keystrokes : filteredKeystrokes() || "No keystrokes received yet."}
      </div>
    </div>
  );
};

export default Terminal;
