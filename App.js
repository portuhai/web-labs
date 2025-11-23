import { BrowserRouter as Router, Routes, Route } from "react-router-dom";
import Header from "./components/Header";
import NavBar from "./components/NavBar";
import Footer from "./components/Footer";
import Home from "./pages/Home";
import Catalog from "./pages/Catalog";
import ItemPage from "./pages/ItemPage";
import { ItemProvider } from './context/ItemsContext';

function App() {
  return (
    <ItemProvider>
      <Router>
        <Header />
        <NavBar />

        <Routes>
          <Route path="/" element={<Home />} />
          <Route path="/catalog" element={<Catalog />} />
          <Route path="/item/:id" element={<ItemPage />} />
        </Routes>

        <Footer />
      </Router>
    </ItemProvider>
  );
}

export default App;
