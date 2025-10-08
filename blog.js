let articles = JSON.parse(localStorage.getItem("articles")) || [];

const form = document.getElementById("articleForm");
const articlesList = document.getElementById("articlesList");

const searchInput = document.createElement("input");
searchInput.type = "text";
searchInput.placeholder = "🔍 Search articles...";
searchInput.style.marginBottom = "15px";
searchInput.style.padding = "8px";
searchInput.style.width = "100%";
searchInput.style.borderRadius = "8px";
searchInput.style.border = "1px solid #ccc";

const sortSelect = document.createElement("select");
sortSelect.innerHTML = `
  <option value="">Sort by...</option>
  <option value="title_asc">Title A→Z</option>
  <option value="title_desc">Title Z→A</option>
  <option value="length_asc">Shorter → Longer</option>
  <option value="length_desc">Longer → Shorter</option>
`;
sortSelect.style.marginBottom = "20px";
sortSelect.style.padding = "8px";
sortSelect.style.borderRadius = "8px";
sortSelect.style.border = "1px solid #ccc";
sortSelect.style.width = "100%";

const totalDiv = document.createElement("div");
totalDiv.style.textAlign = "center";
totalDiv.style.marginTop = "20px";
totalDiv.style.fontWeight = "600";
totalDiv.style.color = "#444";

const container = document.querySelector(".container");
container.insertBefore(searchInput, articlesList);
container.insertBefore(sortSelect, articlesList);
container.appendChild(totalDiv);


function renderArticles() {
  articlesList.innerHTML = "";

  let filtered = articles.filter(a =>
    a.title.toLowerCase().includes(searchInput.value.toLowerCase()) ||
    a.text.toLowerCase().includes(searchInput.value.toLowerCase())
  );

  const sortBy = sortSelect.value;
  if (sortBy === "title_asc") filtered.sort((a, b) => a.title.localeCompare(b.title));
  if (sortBy === "title_desc") filtered.sort((a, b) => b.title.localeCompare(a.title));
  if (sortBy === "length_asc") filtered.sort((a, b) => a.text.length - b.text.length);
  if (sortBy === "length_desc") filtered.sort((a, b) => b.text.length - a.text.length);

  filtered.forEach((article, index) => {
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

  const totalChars = articles.reduce((sum, a) => sum + a.text.length, 0);
  totalDiv.textContent = `🧮 Total characters in all articles: ${totalChars}`;
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

searchInput.addEventListener("input", renderArticles);
sortSelect.addEventListener("change", renderArticles);

renderArticles();
