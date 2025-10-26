const API_URL = "http://localhost:3000/articles";

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

let articles = [];
let editIndex = null, deleteIndex = null;


async function loadArticles() {
  const res = await fetch(API_URL);
  articles = await res.json();
  renderArticles();
}

async function addArticle(title, text) {
  await fetch(API_URL, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ title, text })
  });
  await loadArticles();
}

async function updateArticle(id, title, text) {
  await fetch(`${API_URL}/${id}`, {
    method: "PUT",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ title, text })
  });
  await loadArticles();
}

async function deleteArticle(id) {
  await fetch(`${API_URL}/${id}`, { method: "DELETE" });
  await loadArticles();
}


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
    articlesList.innerHTML = `<p style="width:100%;text-align:center;color:#777;">No articles found</p>`;
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

  totalInfo.textContent = `Total characters: ${articles.reduce((s, a) => s + a.text.length, 0)}`;
}


form.addEventListener("submit", e => {
  e.preventDefault();
  const title = document.getElementById("title").value.trim();
  const text = document.getElementById("text").value.trim();
  if (!title || !text) return;
  addArticle(title, text);
  form.reset();
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
    const art = articles[editIndex];
    updateArticle(art.id, editTitle.value.trim(), editText.value.trim());
    editModal.style.display = "none";
  }
};

cancelEdit.onclick = () => (editModal.style.display = "none");

function openDeleteModal(i) {
  deleteIndex = i;
  deleteModal.style.display = "flex";
}

confirmDelete.onclick = () => {
  if (deleteIndex !== null) {
    const art = articles[deleteIndex];
    deleteArticle(art.id);
    deleteModal.style.display = "none";
  }
};

cancelDelete.onclick = () => (deleteModal.style.display = "none");

window.onclick = e => {
  if (e.target === editModal) editModal.style.display = "none";
  if (e.target === deleteModal) deleteModal.style.display = "none";
};

loadArticles();
