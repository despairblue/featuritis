open Base;
open Stdio;

// let require = path => {
//   let content = ref("");
//
//     content := In_channel.read_all(file)
//   In_channel.with_file(path, ~f=file => {
//   });
// };

type t = {
  apiUrl: string,
  authToken: option(string),
};

let make = () => {
  apiUrl: "https://en-master.wunderflats.xyz/api/graphql",
  authToken: None,
};

let fromJson = json => {
  Yojson.Basic.Util.{
    apiUrl: json |> member("apiUrl") |> to_string,
    authToken: json |> member("authToken") |> to_string_option,
  };
};

let toJson = config => {
  `Assoc([
    ("apiUrl", `String(config.apiUrl)),
    (
      "authToken",
      config.authToken
      |> Option.map(~f=authToken => `String(authToken))
      |> Option.value(~default=`Null),
    ),
  ]);
};

let readConfig = config => {
  let applicationConfigDir =
    Caml.Filename.concat(Xdg.config_dir, "Featuritis");
  let configPath = Caml.Filename.concat(applicationConfigDir, "config.json");

  printf("configPath: %s\n%!", configPath);

  try(UnixLabels.mkdir(~perm=0o755, applicationConfigDir)) {
  | _firstException => ()
  };

  let json =
    try(Yojson.Basic.from_file(configPath)) {
    | _firstException => make() |> toJson
    };

  Yojson.Basic.Util.{
    apiUrl: json |> member("apiUrl") |> to_string,
    authToken: json |> member("authToken") |> to_string_option,
  };
  // {apiUrl: "", authToken: None};
};

let writeConfig = config => {
  let applicationConfigDir =
    Caml.Filename.concat(Xdg.config_dir, "Featuritis");
  let configPath = Caml.Filename.concat(applicationConfigDir, "config.json");

  printf("configPath: %s\n%!", configPath);

  let configJSON =
    `Assoc([
      ("apiUrl", `String(config.apiUrl)),
      (
        "authToken",
        config.authToken
        |> Option.map(~f=authToken => `String(authToken))
        |> Option.value(~default=`Null),
      ),
    ]);

  Yojson.Basic.to_file(configPath, configJSON);
};
