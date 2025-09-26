let playlist = [];
const audioPlayer = document.getElementById("audioPlayer");

// Add song
function addSong() {
  const name = document.getElementById('name').value.trim();
  const artist = document.getElementById('artist').value.trim();
  const album = document.getElementById('album').value.trim();
  const duration = parseInt(document.getElementById('duration').value, 10);
  const url = document.getElementById('url') ? document.getElementById('url').value.trim() : "";

  if (!name || !artist || !album || !duration || duration <= 0) {
    alert("Please fill all fields with valid values.");
    return;
  }

  playlist.push({ name, artist, album, duration, url });

  clearForm();
  renderPlaylist();
}

// Delete song
function deleteSong(index) {
  playlist.splice(index, 1);
  renderPlaylist();
}

// Shuffle playlist
function shuffle() {
  for (let i = playlist.length - 1; i > 0; i--) {
    const j = Math.floor(Math.random() * (i + 1));
    [playlist[i], playlist[j]] = [playlist[j], playlist[i]];
  }
  renderPlaylist();
}

// Render playlist
function renderPlaylist() {
  const list = document.getElementById("playlist");
  list.innerHTML = "";
  playlist.forEach((song, i) => {
    const li = document.createElement("li");
    li.className = "row";
    li.innerHTML = `
      <div class="info">
        <div class="title">${escapeHtml(song.name)}</div>
        <div class="meta">${escapeHtml(song.artist)} — <em>${escapeHtml(song.album)}</em> · ${song.duration}s</div>
      </div>
      <div class="buttons">
        ${song.url ? `<button onclick="playSong(${i})">Play</button>` : ""}
        <button onclick="deleteSong(${i})">Delete</button>
      </div>
    `;
    list.appendChild(li);
  });
}

// Play song
function playSong(index) {
  if (playlist[index] && playlist[index].url) {
    audioPlayer.src = playlist[index].url;
    audioPlayer.play();
  }
}

// Clear input fields
function clearForm() {
  ['name','artist','album','duration','url'].forEach(id => {
    if(document.getElementById(id)) document.getElementById(id).value = "";
  });
}

// Escape HTML
function escapeHtml(str) {
  return String(str).replace(/[&<>"']/g, s => ({
    "&":"&amp;","<":"&lt;",">":"&gt;","\"":"&quot;","'":"&#39;"
  }[s]));
}

// Auto-load empty playlist
renderPlaylist();
