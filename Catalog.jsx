import { useItems } from "../context/ItemsContext";
import { useState } from "react";
import ArticleCard from "../components/ArticleCard";
import "./Catalog.css";

export default function Catalog() {
  const { items } = useItems();

  const [search, setSearch] = useState("");
  const [category, setCategory] = useState("");
  const [color, setColor] = useState("");

  const filtered = items.filter((item) => {
    return (
      item.title.toLowerCase().includes(search.toLowerCase()) &&
      (category ? item.category === category : true) &&
      (color ? item.color === color : true)
    );
  });

  return (
    <div className="catalog">
      <h2 className="catalog-title">Catalog</h2>

      <input
        className="search"
        placeholder="Search articles..."
        value={search}
        onChange={(e) => setSearch(e.target.value)}
      />

      <div className="filters">
        <select onChange={(e) => setCategory(e.target.value)}>
          <option value="">Category</option>
          <option value="fashion">Fashion</option>
          <option value="lifestyle">Lifestyle</option>
          <option value="beauty">Beauty</option>
        </select>

        <select onChange={(e) => setColor(e.target.value)}>
          <option value="">Color</option>
          <option value="pink">Pink</option>
          <option value="white">White</option>
          <option value="black">Black</option>
        </select>
      </div>

      <div className="catalog-grid">
        {filtered.map((item) => (
          <ArticleCard key={item.id} item={item} />
        ))}
      </div>
    </div>
  );
}
