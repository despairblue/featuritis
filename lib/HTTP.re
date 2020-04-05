open Base;
open Stdio;

// let api_url = "https://en-master.wunderflats.xyz/api/graphql";
// let api_url = "https://wunderflats.com/api/graphql";
// let api_url = "http://localhost:3000/api/v0.0.1/graphql";

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

let sendQuery = (~query, ~variables, ~authToken, ~apiUrl) => {
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

  let.flatMapSome {Response.body, _} = post(apiUrl, ~headers, ~body);

  Some(
    body
    |> Response.Body.toString
    |> Yojson.Basic.from_string
    |> Yojson.Basic.Util.member("data"),
  );
};

let sendPost = (~email, ~password) => {
  open Fetch;

  printf("sendPost\n%!");

  let headers = [("content-type", "application/json"), ("Accept", "*/*")];
  let body =
    Yojson.Basic.to_string(
      `Assoc([
        ("email", `String(email)),
        ("password", `String(password)),
      ]),
    );

  printf("body: %s\n%!", body);

  let api_url = "https://en-master.wunderflats.xyz/api/auth/login";
  let.flatMapSome {Response.body, _} = post(api_url, ~headers, ~body);

  let foo = body |> Response.Body.toString;

  printf("%s\n%!", foo);

  Some(
    body
    |> Response.Body.toString
    |> Yojson.Basic.from_string
    |> Yojson.Basic.Util.member("authToken")
    |> Yojson.Basic.Util.to_string,
  );
};
