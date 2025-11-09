import React from "react";
import Header from "./components/Header";
import NavBar from "./components/NavBar";
import ArticleList from "./components/ArticleList";
import Footer from "./components/Footer";
import "./App.css";

function App() {
  return (
    <div className="app">
      <Header />
      <NavBar />
      <main className="content">
        <ArticleList />
      </main>
      <Footer />
    </div>
  );
}

export default App;
