open Base;
open Stdio;

type t = {
  state: State.t,
  dispatch: State.action => unit,
};

let make = (state, dispatch) => {state, dispatch};
