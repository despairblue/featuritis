open Base;
open Stdio;

let sendQuery = () => {
  print_endline("sendQuery");
  Fetch.(
    get("https://google.com")
    |> Lwt.map(
         fun
         | Ok({Response.body, headers, status, url}) => {
             printf(
               "Headers: \n%sStatus-Code: %d\nBody: %s\nUrl: %s\n\n",
               Response.Headers.toString(headers),
               Response.Status.toCode(status),
               Response.Body.toString(body),
               url,
             );

             Some(body |> Response.Body.toString |> String.length);
           }
         | Error(e) => {
             printf("That's an error: %s", e);
             None;
           },
       )
  );
};
