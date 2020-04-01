open Base;
open Stdio;

let (let.flatMapSome) = (promise, fn) =>
  Lwt.map(
    fun
    | Ok(response) => fn(response)
    | Error(error) => {
        printf("That's and error: %s%!", error);

        None;
      },
    promise,
  );

let sendQuery = (~query, ~variables, ~authToken) => {
  open Fetch;

  printf("sendQuery\n%!");

  let query =
    `Assoc([
      ("operationName", `Null),
      ("query", `String(query)),
      ("variables", variables),
    ]);
  let body = Yojson.Basic.to_string(query);
  let headers = [
    ("authToken", authToken),
    ("content-type", "application/json"),
    ("Accept", "*/*"),
  ];

  let.flatMapSome {Response.body, _} =
    post("https://en-master.wunderflats.xyz/api/graphql", ~headers, ~body);
  // post("https://wunderflats.com/api/graphql", ~headers, ~body);
  // post("http://localhost:3000/api/v0.0.1/graphql", ~headers, ~body);
  // post("http://localhost:3000/api/v0.0.1/graphql", ~headers, ~body);

  Some(
    body
    |> Response.Body.toString
    |> Yojson.Basic.from_string
    |> Yojson.Basic.Util.member("data"),
  );
};
