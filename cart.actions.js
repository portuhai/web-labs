// redux/cart/cart.actions.js
export const ADD_TO_CART = "cart/add";
export const REMOVE_FROM_CART = "cart/remove";
export const INCREASE_QTY = "cart/increase";
export const DECREASE_QTY = "cart/decrease";

export const addToCart = product => ({
  type: ADD_TO_CART,
  payload: product
});

export const removeFromCart = id => ({
  type: REMOVE_FROM_CART,
  payload: id
});

export const increaseQty = id => ({
  type: INCREASE_QTY,
  payload: id
});

export const decreaseQty = id => ({
  type: DECREASE_QTY,
  payload: id
});
