import React from "react";
import ArticleCard from "./ArticleCard";

const articles = [
  {
    title: "How to Find Inspiration in Small Things",
    image: "https://images.unsplash.com/photo-1496307042754-b4aa456c4a2d?w=800",
    summary: "Learn how to stay creative and inspired every day with simple habits.",
  },
  {
    title: "5 Cozy Morning Rituals",
    image: "https://images.unsplash.com/photo-1506744038136-46273834b3fb?w=800",
    summary: "Start your day right with mindfulness and warmth.",
  },
  {
    title: "Writing That Heals the Soul",
    image: "https://images.unsplash.com/photo-1504384308090-c894fdcc538d?w=800",
    summary: "How journaling can transform your mood and mindset.",
  },
];

function ArticleList() {
  return (
    <section className="articles">
      <h2>Featured Articles</h2>
      <div className="article-grid">
        {articles.map((a, index) => (
          <ArticleCard
            key={index}
            title={a.title}
            image={a.image}
            summary={a.summary}
          />
        ))}
      </div>
    </section>
  );
}

export default ArticleList;
