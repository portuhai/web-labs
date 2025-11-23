import "./ArticleCard.css";

export default function ArticleCard({ item }) {
  return (
    <div className="card">
      <img
        src={item.image}
        alt={item.title}
        className="card-img"
      />
      <h3 className="card-title">{item.title}</h3>
      <p className="card-text">{item.description}</p>

      <button className="card-btn">View more</button>
    </div>
  );
}
