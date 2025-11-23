import { useParams } from "react-router-dom";
import { useItems } from "../context/ItemsContext";

export default function ItemPage() {
  const { id } = useParams();
  const { items } = useItems();

  const item = items.find(i => i.id === Number(id));

  if (!item) return <h2>Item not found</h2>;

  return (
    <div style={{ padding: "20px" }}>
      <h1>{item.title}</h1>

      <img
        src={item.image}
        alt={item.title}
        style={{ width: "400px", borderRadius: "10px", marginBottom: "20px" }}
      />

      <p><strong>Description:</strong> {item.description}</p>
      <p><strong>Type:</strong> {item.type}</p>
      <p><strong>Color:</strong> {item.color}</p>
      <p><strong>Size:</strong> {item.size}</p>
    </div>
  );
}
