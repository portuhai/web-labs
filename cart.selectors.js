export const selectCartItems = state => state.cart.items;

export const selectTotalPrice = state =>
  state.cart.items.reduce(
    (sum, i) => sum + i.product.price * i.quantity,
    0
  );
export const selectCartCount = state =>
  state.cart.items.reduce(
    (sum, item) => sum + item.quantity,
    0
  );