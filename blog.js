let articles = JSON.parse(localStorage.getItem("articles")) || [];

const form = document.getElementById("articleForm");
const articlesList = document.getElementById("articlesList");

function renderArticles() {
  articlesList.innerHTML = "";
  articles.forEach((article, index) => {
    const div = document.createElement("div");
    div.className = "article";
    div.innerHTML = `
      <span>${article.title}</span>
      <p>${article.text}</p>
      <div class="actions">
        <button onclick="editArticle(${index})"> Edit</button>
        <button onclick="deleteArticle(${index})"> Delete</button>
      </div>
    `;
    articlesList.appendChild(div);
  });
  localStorage.setItem("articles", JSON.stringify(articles));
}

form.addEventListener("submit", e => {
  e.preventDefault();
  const title = document.getElementById("title").value.trim();
  const text = document.getElementById("text").value.trim();

  if (title && text) {
    articles.push({ title, text });
    form.reset();
    renderArticles();
  }
});

function editArticle(index) {
  const newTitle = prompt("Edit title:", articles[index].title);
  const newText = prompt("Edit text:", articles[index].text);
  if (newTitle && newText) {
    articles[index] = { title: newTitle, text: newText };
    renderArticles();
  }
}

function deleteArticle(index) {
  if (confirm("Are you sure you want to delete this article?")) {
    articles.splice(index, 1);
    renderArticles();
  }
}

renderArticles();

