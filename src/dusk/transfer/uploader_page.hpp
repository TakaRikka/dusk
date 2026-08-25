#ifndef DUSK_TRANSFER_UPLOADER_PAGE_HPP
#define DUSK_TRANSFER_UPLOADER_PAGE_HPP

#include <string_view>

namespace dusk::transfer {

// Served verbatim by GET /, with %%ACCEPTED_GAME_IDS%% substituted for the catalog's ids. Must stay
// self-contained -- no external CSS, fonts or scripts -- because the Apple TV serving it has no
// route the browser could fetch them from.
//
// The pre-flight below is a UX guard only: it exists so a wrong file costs nothing instead of a
// multi-minute upload. The device's own dusk::iso::validate is what authorises publishing.
inline constexpr std::string_view kUploaderPage = R"PAGE(<!doctype html>
<html lang="en">
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>Add a disc</title>
<style>
  :root { color-scheme: light dark; --fg:#111; --bg:#fff; --muted:#666; --line:#d8d8d8;
          --accent:#2b6cb0; --bad:#a3271f; --good:#256d3a; }
  @media (prefers-color-scheme: dark) {
    :root { --fg:#eee; --bg:#141414; --muted:#9a9a9a; --line:#333; --accent:#7cb0e0;
            --bad:#e88a80; --good:#7fc99a; }
  }
  * { box-sizing: border-box; }
  body { margin:0; padding:24px; background:var(--bg); color:var(--fg);
         font:16px/1.5 -apple-system,BlinkMacSystemFont,"Segoe UI",Roboto,sans-serif; }
  main { max-width:34rem; margin:0 auto; }
  h1 { font-size:1.4rem; margin:0 0 .25rem; }
  p.sub { color:var(--muted); margin:0 0 1.5rem; }
  label.pick { display:block; border:2px dashed var(--line); border-radius:12px; padding:28px 16px;
               text-align:center; cursor:pointer; }
  label.pick:focus-within { border-color:var(--accent); }
  input[type=file] { position:absolute; width:1px; height:1px; opacity:0; }
  button { font:inherit; padding:12px 20px; border-radius:10px; border:1px solid var(--line);
           background:var(--accent); color:#fff; cursor:pointer; margin-top:1rem; }
  button[disabled] { opacity:.5; cursor:default; }
  .bar { height:10px; border-radius:5px; background:var(--line); overflow:hidden; margin:1rem 0 .35rem; }
  .bar > i { display:block; height:100%; width:0; background:var(--accent); transition:width .2s; }
  .row { display:flex; justify-content:space-between; color:var(--muted); font-size:.9rem; }
  .msg { margin-top:1rem; padding:12px 14px; border-radius:10px; border:1px solid var(--line); }
  .msg.bad { color:var(--bad); border-color:var(--bad); }
  .msg.good { color:var(--good); border-color:var(--good); }
  .hide { display:none; }
</style>
<main>
  <h1>Add a disc</h1>
  <p class="sub">Choose your disc image. It is checked before anything is sent.</p>

  <label class="pick">
    <input type="file" id="file" accept=".iso,.gcm,.ciso,.gcz,.nfs,.rvz,.wbfs,.wia,.tgc">
    <span id="picked">Tap to choose a file</span>
  </label>

  <button id="go" disabled>Upload</button>

  <div id="prog" class="hide">
    <div class="bar"><i id="fill"></i></div>
    <div class="row"><span id="pct">0%</span><span id="bytes"></span></div>
  </div>

  <div id="msg" class="msg hide"></div>
</main>
<script>
const ACCEPTED_GAME_IDS = %%ACCEPTED_GAME_IDS%%;
const CHUNK = 8388608;
const EXTS = ["iso","gcm","ciso","gcz","nfs","rvz","wbfs","wia","tgc"];
// Container magic numbers, so an obviously wrong file is caught without uploading it. The real
// hash cannot be reproduced here -- the app hashes the decoded disc, not the file.
const MAGIC = {
  rvz:  [0x52,0x56,0x5A,0x01],
  wia:  [0x57,0x49,0x41,0x01],
  ciso: [0x43,0x49,0x53,0x4F],
  wbfs: [0x57,0x42,0x46,0x53],
  gcz:  [0x01,0xC0,0x0B,0xB1],
};

const $ = (id) => document.getElementById(id);
let chosen = null;

function say(text, kind) {
  const el = $("msg");
  el.textContent = text;
  el.className = "msg" + (kind ? " " + kind : "");
}
function human(n) {
  const u = ["B","KB","MB","GB"];
  let i = 0;
  while (n >= 1024 && i < u.length - 1) { n /= 1024; i++; }
  return n.toFixed(i === 0 ? 0 : 1) + " " + u[i];
}
async function head(file, len) {
  return new Uint8Array(await file.slice(0, len).arrayBuffer());
}

async function preflight(file) {
  const ext = (file.name.split(".").pop() || "").toLowerCase();
  if (!EXTS.includes(ext)) {
    return { ok: false, msg: "That is not a disc image. Expected one of: " + EXTS.join(", ") + "." };
  }
  if (ext === "iso" || ext === "gcm") {
    const b = await head(file, 6);
    if (b.length < 6) return { ok: false, msg: "That file is too small to be a disc image." };
    const id = String.fromCharCode.apply(null, b);
    if (!/^[A-Z0-9]{6}$/.test(id)) {
      return { ok: false, msg: "That does not look like a disc image." };
    }
    if (ACCEPTED_GAME_IDS.length && ACCEPTED_GAME_IDS.indexOf(id) === -1) {
      return { ok: false, msg: "This disc is " + id + ", which this app does not support." };
    }
    return { ok: true, note: "Recognised " + id + "." };
  }
  const want = MAGIC[ext];
  if (want) {
    const b = await head(file, 4);
    for (let i = 0; i < want.length; i++) {
      if (b[i] !== want[i]) {
        return { ok: false, msg: "That file does not look like a valid ." + ext + " image." };
      }
    }
  }
  // Compressed containers cannot be identified further in a browser; the device verifies after upload.
  return { ok: true, note: "Looks like a ." + ext + " image. The Apple TV will verify it after upload." };
}

function setProgress(done, total) {
  $("prog").classList.remove("hide");
  const pct = total ? Math.floor((done / total) * 100) : 0;
  $("fill").style.width = pct + "%";
  $("pct").textContent = pct + "%";
  $("bytes").textContent = human(done) + " of " + human(total);
}

async function upload(file) {
  const q = "name=" + encodeURIComponent(file.name) + "&size=" + file.size;
  let res = await fetch("/status?" + q);
  if (res.status === 409) {
    say("Another upload is already in progress on this Apple TV.", "bad");
    return;
  }
  if (!res.ok) { say("Could not start the upload.", "bad"); return; }
  let state = await res.json();
  let sent = state.received;
  if (sent > 0) say("Resuming from " + human(sent) + ".", null);
  setProgress(sent, file.size);

  while (sent < file.size) {
    const end = Math.min(sent + CHUNK, file.size);
    res = await fetch("/chunk?id=" + state.id + "&offset=" + sent, {
      method: "POST",
      body: file.slice(sent, end),
    });
    if (res.status === 507) { say("The Apple TV is out of space.", "bad"); return; }
    let body = null;
    try { body = await res.json(); } catch (e) { body = null; }
    if (!res.ok) {
      // The server hands back what it actually holds; follow that rather than guessing.
      if (body && typeof body.received === "number") { sent = body.received; continue; }
      say("The upload was interrupted. Press Upload to resume.", "bad");
      return;
    }
    // Advance by what the server acknowledges, never by what we sent.
    sent = body.received;
    setProgress(sent, file.size);
  }

  say("Verifying on the Apple TV. This can take a few minutes for a large disc.", null);
  res = await fetch("/finalize?id=" + state.id, { method: "POST" });
  const done = await res.json();
  if (done.ok) {
    say("Done. The disc is now available on the Apple TV.", "good");
  } else {
    say(done.reason || "Verification failed.", "bad");
  }
}

$("file").addEventListener("change", async (e) => {
  chosen = e.target.files[0] || null;
  $("go").disabled = true;
  if (!chosen) { $("picked").textContent = "Tap to choose a file"; return; }
  $("picked").textContent = chosen.name + " (" + human(chosen.size) + ")";
  const verdict = await preflight(chosen);
  if (!verdict.ok) { say(verdict.msg, "bad"); chosen = null; return; }
  say(verdict.note, null);
  $("go").disabled = false;
});

$("go").addEventListener("click", async () => {
  if (!chosen) return;
  $("go").disabled = true;
  try { await upload(chosen); }
  catch (err) { say("The upload stopped: " + err + " Press Upload to resume.", "bad"); }
  $("go").disabled = false;
});
</script>
</html>
)PAGE";

}  // namespace dusk::transfer

#endif  // DUSK_TRANSFER_UPLOADER_PAGE_HPP
