import express from "express";
import fs from "fs";
import cors from "cors";

const app = express();
const PORT = 3000;
const DATA_FILE = "C:/Users/User/PycharmProjects/PythonProject/веб/back-end/data.json";

app.use(cors());
app.use(express.json());

// якщо файлу немає — створюємо
if (!fs.existsSync(DATA_FILE)) fs.writeFileSync(DATA_FILE, "[]");

function readData() {
  const text = fs.readFileSync(DATA_FILE, "utf8");
  return text ? JSON.parse(text) : [];
}

function writeData(data) {
  fs.writeFileSync(DATA_FILE, JSON.stringify(data, null, 2));
}

app.get("/articles", (req, res) => {
  res.json(readData());
});

app.post("/articles", (req, res) => {
  const articles = readData();
  const newArticle = { id: Date.now(), ...req.body };
  articles.push(newArticle);
  writeData(articles);
  res.status(201).json(newArticle);
});

app.put("/articles/:id", (req, res) => {
  let articles = readData();
  const id = parseInt(req.params.id);
  const index = articles.findIndex(a => a.id === id);
  if (index === -1) return res.status(404).send("Not found");
  articles[index] = { ...articles[index], ...req.body };
  writeData(articles);
  res.json(articles[index]);
});

app.delete("/articles/:id", (req, res) => {
  let articles = readData();
  const id = parseInt(req.params.id);
  articles = articles.filter(a => a.id !== id);
  writeData(articles);
  res.json({ message: "Deleted" });
});

app.listen(PORT, () =>
  console.log(`✅ Server running on http://localhost:${PORT}`)
);
