let str = ReasonReact.string;

[@bs.module] external gql : ReasonApolloTypes.gql = "graphql-tag";

[@bs.send]
external query :
  (ApolloClient.generatedApolloClient, ReasonApolloTypes.queryString) =>
  Js.Promise.t('c) =
  "query";

type state = {client: ApolloClient.generatedApolloClient};

type action =
  | NoUpdate;

let component = ReasonReact.reducerComponent("RqlTest");

module CustomQuery = [%graphql {|
    query {
      hello
    }
  |}];

let queryThing = CustomQuery.make();

let customQuery = gql(. CustomQuery.make()##query);

let externalQuery = [%bs.raw {|require("../gqlQueries/helloWorld.query.js")|}];

let makeConnection = () : ApolloClient.generatedApolloClient =>
  ReasonApollo.createApolloClient(
    ~link=
      ApolloLinks.createHttpLink(
        ~uri="https://q80vw8qjp.lp.gql.zone/graphql",
        (),
      ),
    ~cache=ApolloInMemoryCache.createInMemoryCache(),
    (),
  );

let make = _children => {
  ...component,
  initialState: () => {client: makeConnection()},
  didMount: self => {
    Js.log(customQuery);
    Js.log(queryThing);
    Js.log(externalQuery);
    query(self.state.client, customQuery)
    |> Js.Promise.then_(res => {
         Js.log(res);
         Js.Promise.resolve();
       })
    |> ignore;
  },
  reducer: (action, _state) =>
    switch (action) {
    | NoUpdate => ReasonReact.NoUpdate /* need a reducer */
    },
  render: _self => <div> <h1> (str("Re Component")) </h1> </div>,
};