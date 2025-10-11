let articles = JSON.parse(localStorage.getItem("articles")) || [];
const form = document.getElementById("articleForm");
const articlesList = document.getElementById("articlesList");
const searchInput = document.getElementById("searchInput");
const sortSelect = document.getElementById("sortSelect");
const totalInfo = document.getElementById("totalInfo");

const editModal = document.getElementById("editModal");
const deleteModal = document.getElementById("deleteModal");
const editTitle = document.getElementById("editTitle");
const editText = document.getElementById("editText");
const saveEdit = document.getElementById("saveEdit");
const cancelEdit = document.getElementById("cancelEdit");
const confirmDelete = document.getElementById("confirmDelete");
const cancelDelete = document.getElementById("cancelDelete");

let editIndex = null, deleteIndex = null;

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
  if (filtered.length === 0) {
    articlesList.innerHTML = `<p style="width:100%;text-align:center;color:#777;">No articles found 🗞️</p>`;
  }
  filtered.forEach((article, index) => {
    const div = document.createElement("div");
    div.className = "article";
    div.innerHTML = `
      <span>${article.title}</span>
      <p>${article.text}</p>
      <div class="actions">
        <button onclick="openEditModal(${index})">Edit</button>
        <button onclick="openDeleteModal(${index})">Delete</button>
      </div>`;
    articlesList.appendChild(div);
  });
  localStorage.setItem("articles", JSON.stringify(articles));
  totalInfo.textContent = `Total characters: ${articles.reduce((s, a) => s + a.text.length, 0)}`;
}

form.addEventListener("submit", e => {
  e.preventDefault();
  const title = document.getElementById("title").value.trim();
  const text = document.getElementById("text").value.trim();
  if (!title || !text) return;
  articles.push({ title, text });
  form.reset();
  renderArticles();
});

searchInput.addEventListener("input", renderArticles);
sortSelect.addEventListener("change", renderArticles);

function openEditModal(i) {
  editIndex = i;
  editTitle.value = articles[i].title;
  editText.value = articles[i].text;
  editModal.style.display = "flex";
}

saveEdit.onclick = () => {
  if (editIndex !== null) {
    articles[editIndex].title = editTitle.value.trim();
    articles[editIndex].text = editText.value.trim();
    editModal.style.display = "none";
    renderArticles();
  }
};

cancelEdit.onclick = () => (editModal.style.display = "none");

function openDeleteModal(i) {
  deleteIndex = i;
  deleteModal.style.display = "flex";
}

confirmDelete.onclick = () => {
  if (deleteIndex !== null) {
    articles.splice(deleteIndex, 1);
    deleteModal.style.display = "none";
    renderArticles();
  }
};

cancelDelete.onclick = () => (deleteModal.style.display = "none");

window.onclick = e => {
  if (e.target === editModal) editModal.style.display = "none";
  if (e.target === deleteModal) deleteModal.style.display = "none";
};

renderArticles();
