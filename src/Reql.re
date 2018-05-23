[@bs.module] external gql : ReasonApolloTypes.gql = "graphql-tag";

[@bs.send]
external query : (ApolloClient.generatedApolloClient, 'a) => Js.Promise.t('b) =
  "query";

let prepareQuery = q => {
  "query": gql(. q##query),
  "parse": q##parse,
  "variables": q##variables,
};