type t('a) =
  | Idle
  | Loading
  | Error(string)
  | Data('a);

let stringOfRequestState =
  fun
  | Idle => "Idle"
  | Loading => "Loading"
  | Error(_) => "Error"
  | Data(_) => "Data";

let map = (request, ~f) => {
  switch (request) {
  | Idle => Idle
  | Loading => Loading
  | Error(error) => Error(error)
  | Data(data) => Data(f(data))
  };
};
