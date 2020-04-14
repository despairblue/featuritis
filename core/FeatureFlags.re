type featureFlags = {
  name: string,
  enabled: bool,
};

module Query = [%graphql
  {|
    query featureFlags($type_: FeatureFlagsType!, $state: FeatureFlagState!) {
      featureFlags (type: $type_, state: $state) @bsRecord {
        name
        enabled
      }
    }
  |}
];

module GraphqlHooksConfig = {
  let config: ReveryGraphqlHooks.config = {
    baseUrl: "https://en-master.wunderflats.xyz/api/graphql",
    headers: [
      (
        "authToken",
        "fba8ebc3d0827bafb46def017f9a98d7b061bed9a671ede53de69171ed6683ca29cf36cff4a7718ae6d4bc5b7e9f701d8f643cf0ff0d542aea9df4187898d9d7LhOFXNL6T",
      ),
    ],
  };
};
module Graphql = ReveryGraphqlHooks.Make(GraphqlHooksConfig);

module QueryHook = Graphql.Query.Make(Query);

module EnableFeatureFlag = [%graphql
  {|
mutation activateGlobalFeatureFlags(
  $name: [String!]!
  $type_: FeatureFlagsType!
  $state: FeatureFlagState!
) {
  activateGlobalFeatureFlags(input: { names: $name }) {
    ... on ActivateGlobalFeatureFlagsPayloadError {
      message
    }
    ... on ActivateGlobalFeatureFlagsPayloadSuccess {
      featureFlags(type: $type_, state: $state) @bsRecord {
        name
        enabled
      }
    }
  }
}
  |}
];

module DisableFeatureFlag = [%graphql
  {|
mutation deactivateGlobalFeatureFlags(
  $name: [String!]!
  $type_: FeatureFlagsType!
  $state: FeatureFlagState!
) {
  deactivateGlobalFeatureFlags(input: { names: $name }) {
    ... on DeactivateGlobalFeatureFlagsPayloadError {
      message
    }
    ... on DeactivateGlobalFeatureFlagsPayloadSuccess {
      featureFlags(type: $type_, state: $state) @bsRecord {
        name
        enabled
      }
    }
  }
}
  |}
];
