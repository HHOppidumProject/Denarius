import './App.css';
import {
    BrowserRouter as Router,
    Routes,
    Route,
} from "react-router-dom";
import React, { Suspense, lazy } from 'react';

export default function App() {
    return (
        <Router>
            <Suspense fallback={<div>Loading...</div>}>
                <div>
                    
                    {/* A <Switch> looks through its children <Route>s and
            renders the first one that matches the current URL. */}
                    <Routes>
                        <Route path="/about" element={<About />} />
                        <Route path="/users" element={<Users />} />
                        <Route path="/" element={< Startup />} />
                        <Route path="/index" element={<Home />} />
                        <Route path="/home" element={<Home />} />
                    </Routes>
                </div>
            </Suspense>
        </Router>
    );
}

const Startup = lazy(() => import("./routes/startup"));

const Home = lazy(() => import("./routes/home"));

function About() {
    return <h2>About</h2>;
}

function Users() {
    return <h2>Users</h2>;
}
