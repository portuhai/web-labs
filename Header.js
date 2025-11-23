import "./Header.css";
import { Link } from "react-router-dom";

export default function Header() {
  return (
    <header className="header">
      <div className="header-container">
        <h1 className="logo">PinkPress</h1>

        <nav className="nav">
          <Link to="/">Home</Link>
          <Link to="/catalog">Catalog</Link>
        </nav>
      </div>
    </header>
  );
}
