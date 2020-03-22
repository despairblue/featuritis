open Base;
open Stdio;

let (let.flatMapSome) = (promise, fn) =>
  Lwt.map(
    fun
    | Ok(response) => fn(response)
    | Error(_e) => None,
    promise,
  );

let sendQuery = () => {
  open Fetch;

  let.flatMapSome {Response.body, _} = get("https://google.com");

  Some(body |> Response.Body.toString |> String.length);
};
