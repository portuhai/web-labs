document.addEventListener('DOMContentLoaded', () => {
  const els = document.querySelectorAll('.testimonials_descr');

  if (!els.length) {
    console.warn('Typewriter: no elements found with .testimonials_descr');
    return;
  }

  els.forEach(el => {
    const originalText = (el.dataset.text && el.dataset.text.trim()) || el.textContent.trim();


    const speed = parseInt(el.dataset.speed, 10) || 60;

    if (!originalText) return;

    el.textContent = '';

    let i = 0;
    function step() {
      if (i < originalText.length) {
        el.textContent += originalText.charAt(i);
        i++;
        setTimeout(step, speed);
      } else {
      }
    }
    step();
  });
});
