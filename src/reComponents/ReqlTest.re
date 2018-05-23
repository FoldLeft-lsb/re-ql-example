let str = ReasonReact.string;

type module_query_t = {
  .
  "parse": Js.Json.t => {. "hello": option(string)},
  "query": ReasonApolloTypes.queryString,
  "variables": Js.Json.t,
};

[@bs.module] external gql : ReasonApolloTypes.gql = "graphql-tag";

[@bs.send]
external query :
  (ApolloClient.generatedApolloClient, module_query_t) => Js.Promise.t('a) =
  "query";

type state_t = {client: ApolloClient.generatedApolloClient};

type action_t =
  | NoUpdate;

let component = ReasonReact.reducerComponent("ReqlTest");

module ModuleQuery = [%graphql {|
     {
      hello
    }
  |}];

let moduleQuery = ModuleQuery.make();

let prepareQuery = q : module_query_t => {
  "query": gql(. q##query),
  "parse": q##parse,
  "variables": q##variables,
};

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
  didMount: self =>
    query(self.state.client, prepareQuery(moduleQuery))
    |> Js.Promise.then_(res => {
         Js.log("Re didMount: T");
         Js.log(res##data);
         Js.Promise.resolve();
       })
    |> ignore,
  reducer: (action, _state) =>
    switch (action) {
    | NoUpdate => ReasonReact.NoUpdate
    },
  render: _self => <div> <h1> (str("Re Component")) </h1> </div>,
};