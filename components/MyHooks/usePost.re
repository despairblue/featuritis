open Printf;
open Revery.UI.React;

type t('responseType) = 'responseType;

type state('responseType) =
  | Idle
  | Loading
  | ServerError(Fetch.Response.t)
  | NetworkError(string)
  | Data(t('responseType));

type action('responseType) =
  | SendRequest
  | ServerError(Fetch.Response.t)
  | NetworkError(string)
  | Data('responseType);

let defaultJSONHeaders = [
  ("content-type", "application/json"),
  ("Accept", "application/json"),
];

let reducer = (action, _state) => {
  switch (action) {
  | SendRequest => Loading
  | ServerError(error) => ServerError(error)
  | NetworkError(error) => NetworkError(error)
  | Data(data) => Data(data)
  };
};

let initialState = Idle;

let use =
    (
      ~headers: list(Fetch.Headers.t)=[],
      ~bodyParser=Fun.id,
      ~onCompleted=ignore,
      url: string,
    ) => {
  let%hook (state, dispatch) = Hooks.reducer(~initialState, reducer);

  let sendRequest = (~body: Yojson.Basic.t, ()) => {
    dispatch(SendRequest);

    Fetch.post(~body=Yojson.Basic.to_string(body), ~headers, url)
    |> Lwt.map(result => {
         Fetch.(
           switch (result) {
           | Ok(response) =>
             let Fetch.Response.{body, status} = response;
             let responseBody = Body.toString(body);

             if (Fetch.Status.isSuccessful(status)) {
               let parsedBody = bodyParser(responseBody);

               onCompleted(Data(parsedBody));
               dispatch(Data(parsedBody));
             } else {
               onCompleted(ServerError(response));
               dispatch(ServerError(response));
             };

           | Error(error) =>
             onCompleted(NetworkError(error));
             dispatch(NetworkError(error));
           }
         )
       })
    |> Lwt.on_failure(_, error =>
         dispatch(
           NetworkError(
             "Something so nefarious happened, I have to stay silent to salvage my sanity.",
           ),
         )
       );
  };

  (sendRequest, state);
};
