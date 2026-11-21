(() => {
  const toggle = document.querySelector("[data-nav-toggle]");
  if (toggle) {
    toggle.addEventListener("click", () => {
      document.body.classList.toggle("nav-open");
      toggle.setAttribute("aria-expanded", document.body.classList.contains("nav-open") ? "true" : "false");
    });
  }

  document.querySelectorAll(".sidebar a").forEach((link) => {
    link.addEventListener("click", () => document.body.classList.remove("nav-open"));
  });

  document.querySelectorAll("[data-copy]").forEach((button) => {
    button.addEventListener("click", async () => {
      const target = document.querySelector(button.dataset.copy);
      if (!target) return;
      try {
        await navigator.clipboard.writeText(target.innerText);
        const old = button.textContent;
        button.textContent = "COPIED ✓";
        setTimeout(() => { button.textContent = old; }, 1300);
      } catch (_) {
        button.textContent = "SELECT THE CODE";
      }
    });
  });
})();