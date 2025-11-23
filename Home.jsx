import "./Home.css";
import { useState } from "react";
import { useItems } from "../context/ItemsContext";
import ArticleCard from "../components/ArticleCard";

const Home = () => {
  const { items } = useItems();

  const [visibleCount, setVisibleCount] = useState(6);

  const handleViewMore = () => {
    setVisibleCount(prev => prev + 6);
  };

  const visibleItems = items.slice(0, visibleCount);

  return (
    <div className="home-container">
      <h1 className="page-title">Latest Articles</h1>

      <div className="items-grid">
        {visibleItems.map(item => (
          <ArticleCard key={item.id} item={item} />
        ))}
      </div>

      {visibleCount < items.length && (
        <div className="view-more-container">
          <button className="view-more-btn" onClick={handleViewMore}>
            View More
          </button>
        </div>
      )}
    </div>
  );
};

export default Home;
