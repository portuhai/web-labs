import React from "react";

function ArticleCard({ title, image, summary }) {
  return (
    <div className="article-card">
      <img src={image} alt={title} />
      <h3>{title}</h3>
      <p>{summary}</p>
      <button>Read More</button>
    </div>
  );
}

export default ArticleCard;
