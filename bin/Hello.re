// open Base;
// open Stdio;
//
// let (let.flatMap) = Lwt.bind;
// let (let.flatMapOk) = (promise, fn) =>
//   Lwt.bind(
//     promise,
//     fun
//     | Ok(response) => fn(response)
//     | Error(e) => Lwt.return(Error(e)),
//   );
//
// let (let.flatMapSome) = (promise, fn) =>
//   Lwt.bind(
//     promise,
//     fun
//     | Ok(response) => fn(response)
//     | Error(_e) => Lwt.return(None),
//   );
//
// // Fetch.(
// //   {
// //     let.flatMapOk {Response.body, _} = get("...");
// //     let bodyAsString = Response.Body.toString(body);
// //
// //     Lwt.return(Ok(bodyAsString));
// //   }
// // );
//
// let sendQuery = () => {
//   open Fetch;
//   print_endline("sendQuery");
//   // let.flatMapSome {Response.body, _} = get("https://google.com");
//   // let bodyAsString = Response.Body.toString(body);
//   // Lwt.return(Some(bodyAsString));
//   get("https://google.com")
//   |> Lwt.map(
//        fun
//        | Ok({Response.body, headers, status, url}) => {
//            printf(
//              "Headers: \n%sStatus-Code: %d\nBody: %s\nUrl: %s\n\n%!",
//              Response.Headers.toString(headers),
//              Response.Status.toCode(status),
//              Response.Body.toString(body),
//              url,
//            );
//
//            Some(body |> Response.Body.toString |> String.length);
//          }
//        | Error(e) => {
//            printf("That's an error: %s", e);
//            None;
//          },
//      );
// };
