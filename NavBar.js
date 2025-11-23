import { Link } from "react-router-dom";

export default function NavBar() {
  return (
    <nav style={{
      display: "flex",
      gap: "30px",
      justifyContent: "center",
      padding: "20px",
      background: "#fff",
      boxShadow: "0 2px 6px rgba(0,0,0,0.1)"
    }}>
      <Link to="/">Home</Link>
      <Link to="/catalog">Catalog</Link>
    </nav>
  );
}
