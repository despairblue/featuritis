type featureFlags = {
  name: string,
  enabled: bool,
};

module FeatureFlags = [%graphql
  {|
    {
      featureFlags (type: ALL, state: ENABLED) @bsRecord {
        name
        enabled
      }
    }
  |}
];

open Base;
open Stdio;

let query =
  `Assoc([
    ("operationName", `Null),
    ("query", `String(FeatureFlags.make()#query)),
    ("variables", `Assoc([])),
  ]);
let body = Yojson.Basic.to_string(query);
let headers = [
  (
    "authToken",
    "3670b74d3c6db0ffbc53a5c3760bd369223f2602c434e01cf7d84df34ccf0960cb2dfda7fb72fef46d194ecbee5ea1c0cbfa25280960ebb4f206350228224a55oCGgX9xDG0XP",
  ),
  ("content-type", "application/json"),
  ("Accept", "*/*"),
];

let getFeatureFlags = () => {
  // printf("\n\n\nHUH\n\n\n");
  Fetch.
    // post("https://wunderflats.com/api/graphql", ~headers, ~body)
    (
      post("http://localhost:3000/api/v0.0.1/graphql", ~headers, ~body)
      |> Lwt.map(
           fun
           | Ok({Response.body, _}) => {
               // printf(
               //   "Headers: \n%sStatus-Code: %d\nBody: %s\nUrl: %s",
               //   Response.Headers.toString(headers),
               //   Response.Status.toCode(status),
               //   Response.Body.toString(body),
               //   url,
               // );
               let ffs =
                 body
                 |> Yojson.Basic.from_string
                 |> Yojson.Basic.Util.member("data")
                 |> FeatureFlags.parse
                 |> (response => response#featureFlags);

               // ffs
               // |> Array.iter(~f=ff =>
               //      printf("> Name: %s, Enabled: %b\n", ff.name, ff.enabled)
               //    );

               Some(ffs);
             }
           | Error(e) => {
               printf("That's an error: %s", e);
               None;
             },
         )
    );
};

let sendQuery = getFeatureFlags;

// Fetch.(
//   post("https://wunderflats.com/api/graphql", ~headers, ~body)
//   |> Lwt.map(
//        fun
//        | Ok({Response.body, status, url, headers}) => {
//            printf(
//              "Headers: \n%sStatus-Code: %d\nBody: %s\nUrl: %s",
//              Response.Headers.toString(headers),
//              Response.Status.toCode(status),
//              Response.Body.toString(body),
//              url,
//            );
//            body
//            |> Yojson.Basic.from_string
//            |> Yojson.Basic.Util.member("data")
//            |> FeatureFlags.parse
//            |> (response => response#featureFlags)
//            |> Array.iter(~f=ff =>
//                 printf("> Name: %s, Enabled: %b\n", ff.name, ff.enabled)
//               );
//          }
//        | Error(e) => printf("That's an error: %s", e),
//      )
//   |> Lwt_main.run
// );

// getFeatureFlags() |> Lwt_main.run;
