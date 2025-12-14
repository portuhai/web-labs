// redux/cart/cart.reducer.js
import {
  ADD_TO_CART,
  REMOVE_FROM_CART,
  INCREASE_QTY,
  DECREASE_QTY
} from "./cart.actions";

const initialState = {
  items: []
};

export default function cartReducer(state = initialState, action) {
  switch (action.type) {
    case ADD_TO_CART: {
      const newProduct = action.payload;

      const existing = state.items.find(
        i => i.product.id === newProduct.id && i.product.format === newProduct.format
      );

      if (existing) {
        return {
          ...state,
          items: state.items.map(i =>
            (i.product.id === newProduct.id && i.product.format === newProduct.format)
              ? { ...i, quantity: i.quantity + 1 }
              : i
          )
        };
      }

      return {
        ...state,
        items: [
          ...state.items,
          { product: newProduct, quantity: 1 }
        ]
      };
    }
    case REMOVE_FROM_CART:
      return {
        ...state,
        items: state.items.filter(
          i => i.product.id !== action.payload
        )
      };

    case INCREASE_QTY:
      return {
        ...state,
        items: state.items.map(i =>
          i.product.id === action.payload
            ? { ...i, quantity: i.quantity + 1 }
            : i
        )
      };

    case DECREASE_QTY:
      return {
        ...state,
        items: state.items.map(i =>
          i.product.id === action.payload
            ? { ...i, quantity: Math.max(1, i.quantity - 1) }
            : i
        )
      };

    default:
      return state;
  }
}
